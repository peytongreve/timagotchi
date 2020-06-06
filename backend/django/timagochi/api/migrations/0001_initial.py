# Generated by Django 3.0.5 on 2020-04-30 03:43

from django.db import migrations, models
import django.db.models.deletion
import django.utils.timezone


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Player',
            fields=[
                ('name', models.CharField(max_length=100, primary_key=True, serialize=False)),
                ('health', models.IntegerField(default=100)),
                ('hunger', models.IntegerField(default=0)),
                ('major', models.IntegerField(default=6)),
                ('minor', models.IntegerField()),
                ('happiness', models.IntegerField(default=-20)),
                ('gpa', models.FloatField(default=5.0)),
                ('gpa_slider', models.IntegerField()),
                ('units', models.IntegerField(default=0)),
                ('activity', models.CharField(max_length=100)),
                ('last_seen', models.DateTimeField(default=django.utils.timezone.now)),
            ],
        ),
        migrations.CreateModel(
            name='Course',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('course', models.CharField(max_length=100)),
                ('person', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='person', to='api.Player')),
            ],
        ),
    ]